FROM postgres:16-alpine

COPY certs/server.crt /var/lib/postgresql/server.crt
COPY certs/server.key /var/lib/postgresql/server.key
COPY certs/root.crt   /var/lib/postgresql/root.crt
COPY pg_hba.conf      /var/lib/postgresql/pg_hba.conf

# чтобы постгрес мог читать эти файлики
# а сам server.key может читать только владелец
RUN chown postgres:postgres \
      /var/lib/postgresql/server.crt \
      /var/lib/postgresql/server.key \
      /var/lib/postgresql/root.crt \
      /var/lib/postgresql/pg_hba.conf && \
    chmod 600 /var/lib/postgresql/server.key
