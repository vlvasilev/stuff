FROM alpine:3.15
COPY ./file-curator.sh ./ 
RUN apk add --update bash bc sudo && rm -rf /var/cache/apk/*
ENTRYPOINT [ "./file-curator.sh", "/data/loki/chunks", "10", "15", "300"]