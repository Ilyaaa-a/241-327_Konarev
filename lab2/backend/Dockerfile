FROM python:3.13-slim

# не создавать .pyc файлы и не буферизовать вывод
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

WORKDIR /app

# сначала копируем зависимости и устанавливаем. слой кешируется
# --no-cache-dir не сохранять кеш pip, чтобы образ весил меньше
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt 

# копируем весь исходный код
COPY . .