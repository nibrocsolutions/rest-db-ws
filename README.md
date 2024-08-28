rest-db-ws
Small rest web service written in c. This service uses both microhttpd and mongo c driver packages. The docker build compliles and builds from source.

Commands:
docker build . -t rest-db-ws:latest
docker run -i --rm -p 8080:8080 -d rest-db-ws:latest

http://localhost:8080


https://www.mongodb.com/resources/products/compatibilities/docker

docker pull mongodb/mongodb-community-server:latest
docker run --name mongodb -p 27017:27017 -d mongodb/mongodb-community-server:latest

docker run --name mongodb -d -p 27017:27017 -e MONGO_INITDB_ROOT_USERNAME=user -e MONGO_INITDB_ROOT_PASSWORD=pass mongodb/mongodb-community-server:latest