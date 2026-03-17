package models

type WiFiDTO struct {
	Ssid string `json:"ssid" form:"ssid" query="ssid"`
	Pass string `json:"pass" form:"pass" query="pass"`
}

type Wifi struct {
	ID   uint64 `gorm:"primaryKey"`
	Ssid string
	Pass string
}
