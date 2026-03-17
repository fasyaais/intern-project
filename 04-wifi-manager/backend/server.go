package main

import (
	"backend/models"
	"context"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"time"

	"github.com/labstack/echo/v5"
	"github.com/labstack/echo/v5/middleware"
	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

func main() {

	dsn := "admin:admin@tcp(127.0.0.1:3306)/rw_wifi_manager?charset=utf8mb4&parseTime=True&loc=Local"
	db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	db.AutoMigrate(&models.Wifi{})
	// var clients sync.Map
	client := make(chan string)
	if err != nil {
		log.Fatal(err.Error())
	}

	ctx := context.Background()

	e := echo.New()
	e.Use(middleware.RequestLogger())
	e.Use(middleware.CORS("*"))

	e.GET("/", func(c *echo.Context) error {
		// return c.String(http.StatusOK, "Hello world")
		return c.JSON(http.StatusOK, map[string]string{
			"msg": "success",
		})
	})

	e.HEAD("/health", func(c *echo.Context) error {
		return c.String(http.StatusOK, "")
	})

	e.POST("/", func(c *echo.Context) error {
		ls := new(models.LedStateDTO)

		if err := c.Bind(ls); err != nil {
			fmt.Println(ls)
			return err
		}
		fmt.Println(ls)
		tm := time.Unix(int64(ls.Timestamp), 0)
		err := gorm.G[models.History](db).Create(ctx, &models.History{State: ls.State, Timestamp: &tm})

		if err != nil {
			return err
		}

		return c.JSON(http.StatusCreated, map[string]string{
			"status": "success",
		})
	})

	e.POST("/api/config/", func(c *echo.Context) error {
		req := new(models.ResponseDTO)
		if err := c.Bind(&req); err != nil {
			return err
		}

		if req.Type == "SETTING_WIFI" {
			dat := new(models.WiFiDTO)
			jsonData, _ := json.Marshal(req.Data)
			if err := json.Unmarshal(jsonData, &dat); err != nil {
				return err
			}
			wifi, err := gorm.G[models.Wifi](db).First(ctx)
			if err != nil {
				return err
			}
			wifi.Ssid = dat.Ssid
			wifi.Pass = dat.Pass
			db.Save(&wifi)
			select {
			case client <- string(jsonData):
			default:
				log.Println("No SSE client connected, skipping notification")
			}

		} else if req.Type == "CONFIG_GPIO" {

		}

		return c.JSON(http.StatusCreated, map[string]string{
			"msg": "success",
		})
	})

	e.GET("/sse", func(c *echo.Context) error {
		log.Printf("SSE client connected, ip: %v", c.RealIP())
		w := c.Response()
		w.Header().Set("Content-Type", "text/event-stream")
		w.Header().Set("Cache-Control", "no-cache")
		w.Header().Set("Connection", "keep-alive")
		w.WriteHeader(http.StatusOK)

		ticker := time.NewTicker(3 * time.Second)
		defer ticker.Stop()

		for {
			select {
			case <-c.Request().Context().Done():
				log.Printf("SSE client disconnected, ip: %v", c.RealIP())
				return nil
			case msg := <-client:
				event := models.Event{
					Data:  []byte(msg),
					Event: []byte("SETTING_WIFI"),
				}
				if err := event.MarshalTo(w); err != nil {
					return err
				}
				if err := http.NewResponseController(w).Flush(); err != nil {
					return err
				}
			case <-ticker.C:
				event := models.Event{
					Data: []byte("time: " + time.Now().Format(time.RFC3339Nano)),
				}
				if err := event.MarshalTo(w); err != nil {
					return err
				}
				if err := http.NewResponseController(w).Flush(); err != nil {
					return err
				}
			}
		}
	})

	if err := e.Start("0.0.0.0:3000"); err != nil {
		e.Logger.Error("failed to server", "error", err)
	}
}
