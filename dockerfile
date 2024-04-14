FROM debian:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git

WORKDIR /app

COPY . /app

RUN cmake . && make

CMD ["./producer_consumer"]

