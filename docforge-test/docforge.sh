#!/bin/bash

MANIFEST_FILE=$1
HUGO_CONTENT="hugo/data/$(basename -- $1)"
GIT_OAUTH_TOKEN=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

mkdir -p $HUGO_CONTENT

/Users/i330716/go/src/github.com/gardener/docforge/bin/rel/docforge-darwin-amd64 -f $MANIFEST_FILE -d $HUGO_CONTENT --hugo --github-oauth-token $GIT_OAUTH_TOKEN
# docforge -f $MANIFEST_FILE -d $HUGO_CONTENT --hugo --github-oauth-token $GIT_OAUTH_TOKEN