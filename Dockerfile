FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y wget gnupg lsb-release

RUN wget -qO - https://www.postgresql.org/media/keys/ACCC4CF8.asc | tee /etc/apt/trusted.gpg.d/postgresql.asc

RUN echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" | \
    tee /etc/apt/sources.list.d/pgdg.list

RUN apt-get update && apt-get install -y \
    build-essential \
    postgresql-server-dev-16 \
    libcmocka-dev \
    libpq-dev \
    libselinux1-dev \
    libzstd-dev \
    liblz4-dev \
    libxslt1-dev \
    libpam0g-dev \
    libkrb5-dev \
    zlib1g-dev \
    libreadline-dev \
    git cmake gcc make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /root
COPY . /root/pg_iban
WORKDIR /root/pg_iban

COPY test_pg_iban.c /root/pg_iban/test_pg_iban.c

RUN make test_pg_iban

CMD ./test_pg_iban
