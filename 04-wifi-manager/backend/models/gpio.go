package models

type GpioDTO struct {
	Pin   uint8 `json:"pin" form:"pin" query:"pin"`
	State bool  `json:"state" form:"state" query:"state"`
}
type Gpio struct {
	ID    uint64 `gorm:"primaryKey" json:"-"`
	Pin   uint8  `json:"pin" form:"pin" query:"pin"`
	State bool   `json:"state" form:"state" query:"state"`
}
type Tabler interface {
	TableName() string
}

func (Gpio) TableName() string {
	return "gpio"
}
