FROM debian:bookworm-slim

# Копируем только бинарник
COPY build/linux_build_system /usr/local/bin/build_system

# Устанавливаем рабочую директорию
WORKDIR /app

# Старт по умолчанию
ENTRYPOINT ["build_system"]
