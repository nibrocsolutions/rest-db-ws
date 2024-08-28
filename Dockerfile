FROM alpine:3.12

WORKDIR /tmp
COPY libmicrohttpd-latest.tar.gz /tmp
COPY mongo-c-driver-1.27.4.tar.gz /tmp
WORKDIR /tmp/rest-db-ws
COPY Makefile /tmp/rest-db-ws
COPY src/ /tmp/rest-db-ws/src

RUN addgroup -g 1000 mygroup && \
    mkdir /myuser && \
    adduser -G mygroup -u 1000 -h /myuser -D myuser && \
    chown -R myuser:mygroup /myuser && \
    apk add git build-base tzdata zlib-dev perl cmake openssl openssl-dev && \
    tar xfvz /tmp/libmicrohttpd-latest.tar.gz && \
    (cd libmi*; ./configure; make install) && \
    tar xfvz /tmp/mongo-c-driver-1.27.4.tar.gz && \
    (cd mongo-c*; cmake -S . -B /tmp; cmake; cmake --build /tmp; cmake --install "/tmp" --prefix "/usr/local") && \
    cp -r /usr/local/include/libmongoc-1.0/* /usr/local/include && \
    cp -r /usr/local/include/libbson-1.0/* /usr/local/include && \
    make -C /tmp/rest-db-ws

WORKDIR /myuser

USER myuser

CMD ["/bin/sh"]

FROM alpine:3.12

RUN apk add tzdata 

COPY --from=0 /tmp/rest-db-ws/rest-db-ws /
COPY --from=0 /usr/local/lib/libmicrohttpd.so.* /usr/local/lib
COPY --from=0 /usr/local/lib64/libbson-1.0.so.* /usr/local/lib
COPY --from=0 /usr/local/lib64/libmongoc-1.0.so.* /usr/local/lib

USER 1000 

CMD ["/rest-db-ws"]


