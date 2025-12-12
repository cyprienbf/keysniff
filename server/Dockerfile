FROM golang:alpine AS builder

WORKDIR /app

COPY main.go .

RUN go mod init logserver

RUN go build -o server main.go

FROM alpine:latest

WORKDIR /app

COPY --from=builder /app/server .

EXPOSE 8080

CMD ["./server"]