package services

import (
	"backend/models"
	"backend/repository"
	"context"
	"errors"
	"strconv"

	"gorm.io/gorm"
)

type GPIOService interface {
	GetGPIO(ctx context.Context) ([]models.Gpio, error)
	Create(ctx context.Context, gpio *models.GpioDTO) error
	Find(ctx context.Context, pin string) (models.Gpio, error)
	UpdateState(ctx context.Context, gpio *models.GpioDTO) error
}

type gpioService struct {
	repo repository.GPIORepository
}

func NewGPIOService(repo repository.GPIORepository) GPIOService {
	return &gpioService{
		repo,
	}
}

func (s *gpioService) GetGPIO(ctx context.Context) ([]models.Gpio, error) {
	return s.repo.GetAll(ctx)
}

func (s *gpioService) Create(ctx context.Context, gpio *models.GpioDTO) error {
	// return s.repo.GetAll(ctx)
	data := models.Gpio{
		Pin:   gpio.Pin,
		State: gpio.State,
	}
	return s.repo.Create(ctx, &data)
}

func (s *gpioService) Find(ctx context.Context, pin string) (models.Gpio, error) {
	gpio, err := s.repo.Find(ctx, pin)
	if gpio != (models.Gpio{}) {
		return gpio, nil
	}
	if !errors.Is(err, gorm.ErrRecordNotFound) {
		return models.Gpio{}, err
	}
	pinConv, _ := strconv.Atoi(pin)
	data := models.Gpio{
		Pin:   uint8(pinConv),
		State: false,
	}
	if err := s.repo.Create(ctx, &data); err != nil {
		return models.Gpio{}, err
	}
	return s.repo.Find(ctx, string(data.Pin))
}

func (s *gpioService) UpdateState(ctx context.Context, gpio *models.GpioDTO) error {
	rows, err := s.repo.UpdateState(ctx, gpio)
	if err == nil {
		return err
	}
	if rows <= 0 {
		return errors.New("NOT found")
	}
	return nil
}
