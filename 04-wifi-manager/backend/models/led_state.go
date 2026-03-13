package models

type LedStateDTO struct {
	State     string `json:"state" form:"state" query:"state"`
	Timestamp uint64 `json:"timestamp" form:"timestamp" query:"timestamp"`
}
