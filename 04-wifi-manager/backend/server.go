package main

import (
	"net/http"

	"github.com/labstack/echo/v5"
	"github.com/labstack/echo/v5/middleware"
)

func main() {
	e := echo.New()
	e.Use(middleware.RequestLogger())

	e.GET("/", func(c *echo.Context) error {
		return c.String(http.StatusOK, "Hello world")
	})

	e.POST("/", func(c *echo.Context) error {
		return c.JSON(http.StatusCreated, map[string]string{
			"status": "success",
		})
	})

	if err := e.Start("0.0.0.0:3000"); err != nil {
		e.Logger.Error("failed to server", "error", err)
	}
}
