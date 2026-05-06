FROM nginx:stable-alpine

# удаляем дефолтный конфиг nginx
RUN rm /etc/nginx/conf.d/default.conf

# копируем новый шаблон. nginx сам его обработает при запуске
COPY templates/ /etc/nginx/templates/

# копируем сертификаты
COPY certs/ /etc/nginx/certs/