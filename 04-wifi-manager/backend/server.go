package main

import (
	"backend/models"
	"context"
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
	if err != nil {
		log.Fatal(err.Error())
	}

	ctx := context.Background()

	e := echo.New()
	e.Use(middleware.RequestLogger())

	e.GET("/", func(c *echo.Context) error {
		// return c.String(http.StatusOK, "Hello world")
		return c.JSON(http.StatusOK, map[string]string{
			"msg": "success",
		})
	})

	e.POST("/", func(c *echo.Context) error {
		ls := new(models.LedStateDTO)

		if err := c.Bind(ls); err != nil {
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

	if err := e.Start("0.0.0.0:3000"); err != nil {
		e.Logger.Error("failed to server", "error", err)
	}
}
