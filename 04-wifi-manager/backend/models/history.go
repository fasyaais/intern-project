package models

import "time"

type History struct {
	ID        uint64 `gorm:"primaryKey"`
	State     string
	Timestamp *time.Time
}
