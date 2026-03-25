package handler

import (
	"backend/models"
	"backend/services"
	"backend/utils"
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"net/http"

	"github.com/labstack/echo/v5"
	"gorm.io/gorm"
)

type GPIOHandler interface {
	Get(c *echo.Context) error
	Find(c *echo.Context) error
	Create(c *echo.Context) error
	UpdateState(c *echo.Context) error
}

type gpioHandler struct {
	service services.GPIOService
}

func NewGPIOHandler(service services.GPIOService) GPIOHandler {
	return &gpioHandler{
		service,
	}
}

func (s *gpioHandler) Get(c *echo.Context) error {
	ctx := context.Background()
	data, err := s.service.GetGPIO(ctx)
	if err != nil {
		return err
	}

	return utils.SendJSON(c, http.StatusOK, data, "success to fetch all gpio")
}

func (h *gpioHandler) Find(c *echo.Context) error {
	ctx := context.Background()
	pin := c.Param("pin")
	fmt.Println(pin)
	data, err := h.service.Find(ctx, pin)
	if err != nil {
		if errors.Is(err, gorm.ErrRecordNotFound) {
			return utils.SendJSON(c, http.StatusNotFound, "", "pin not found")
		}
		return err
	}
	json, _ := json.Marshal(data)
	return utils.SendJSON(c, http.StatusOK, string(json), "success to fetch gpio")
}

func (h *gpioHandler) UpdateState(c *echo.Context) error {
	ctx := context.Background()
	// c.Request().Header.Set("Content-Type", "application/json")
	gp := new(models.GpioDTO)
	if err := c.Bind(&gp); err != nil {
		fmt.Println("debuggg")
		return err
	}
	fmt.Println(gp)
	if err := h.service.UpdateState(ctx, gp); err != nil {
		return err
	}
	return utils.SendJSON(c, http.StatusOK, "", "success to update state")
}

func (h *gpioHandler) Create(c *echo.Context) error {
	ctx := context.Background()
	gp := new(models.GpioDTO)

	if err := c.Bind(&gp); err != nil {
		return err
	}
	if err := h.service.Create(ctx, gp); err != nil {
		return err
	}

	return c.JSON(http.StatusOK, gp)
}
