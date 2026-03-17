package models

type ResponseDTO struct {
	Type string `json:"type"`
	Data any    `json:"data"`
}
