FROM alpine:3.15
COPY ./file-generator.sh ./ 
RUN apk add --update bash bc sudo && rm -rf /var/cache/apk/*
ENTRYPOINT [ "./file-generator.sh", "/data/loki/chunks", "91" ]