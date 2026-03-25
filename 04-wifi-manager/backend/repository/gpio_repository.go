package repository

import (
	"backend/models"
	"context"

	"gorm.io/gorm"
)

type GPIORepository interface {
	GetAll(ctx context.Context) ([]models.Gpio, error)
	Create(ctx context.Context, gpio *models.Gpio) error
	Find(ctx context.Context, pin string) (models.Gpio, error)
	UpdateState(ctx context.Context, gpio *models.GpioDTO) (int, error)
}

type gpioRepository struct {
	db *gorm.DB
}

func NewGPIORepository(db *gorm.DB) GPIORepository {
	return &gpioRepository{
		db,
	}
}

func (r *gpioRepository) GetAll(ctx context.Context) ([]models.Gpio, error) {
	data, err := gorm.G[models.Gpio](r.db).Find(ctx)
	if err != nil {
		return nil, err
	}
	return data, nil
}

func (r *gpioRepository) Create(ctx context.Context, gpio *models.Gpio) error {
	return gorm.G[models.Gpio](r.db).Create(ctx, gpio)
}

func (r *gpioRepository) Find(ctx context.Context, pin string) (models.Gpio, error) {
	return gorm.G[models.Gpio](r.db).Where("pin = ?", pin).First(ctx)
}

func (r *gpioRepository) UpdateState(ctx context.Context, gpio *models.GpioDTO) (int, error) {
	return gorm.G[models.Gpio](r.db).Where("pin = ?", gpio.Pin).Limit(1).Update(ctx, "state", gpio.State)
}
