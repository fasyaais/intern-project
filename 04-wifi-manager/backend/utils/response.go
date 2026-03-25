package utils

import "github.com/labstack/echo/v5"

func SendJSON(c *echo.Context, code int, data any, message string) error {
	resp := map[string]any{
		"message": message,
		"data":    data,
	}
	return c.JSON(code, resp)
}
