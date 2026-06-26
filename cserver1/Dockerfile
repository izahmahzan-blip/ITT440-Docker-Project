FROM gcc:latest
RUN apt-get update && apt-get install -y default-libmysqlclient-dev
WORKDIR /app
COPY server.c .
RUN gcc -o server server.c $(mysql_config --cflags --libs)
CMD ["./server"]