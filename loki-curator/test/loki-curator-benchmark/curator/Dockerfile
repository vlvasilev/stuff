# FROM gcc:9.4.0 as builder
FROM gcc:9.4.0
WORKDIR /curator
COPY . .
RUN gcc -O curator.c -o loki-curator && chmod +x loki-curator
ENTRYPOINT ["./loki-curator"]
############# curator #############
# FROM eu.gcr.io/gardener-project/3rd/alpine:3.12.3 AS curator
# COPY --from=builder /curator/loki-curator /opt/curator/
# WORKDIR /opt/curator
# ENTRYPOINT ["./loki-curator"]
