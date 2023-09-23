#!/bin/bash
# Release script run in Github CI


LAST_TAG=$(git describe --tags --abbrev=0)
LAST_TAG_MAJOR=$(echo "$LAST_TAG" | cut -d'.' -f1)
LAST_TAG_MINOR=$(echo "$LAST_TAG" | cut -d'.' -f2)
LAST_TAG_PATCH=$(echo "$LAST_TAG" | cut -d'.' -f3)

git show --pretty=format:%s -s "$LAST_TAG..HEAD" > CHANGELOG

TAG_MAJOR=""
TAG_MINOR=""
TAG_PATCH=""

WARNINGS=""

if [[ "$(git show --pretty=format:%s --grep '^MAJOR' "$LAST_TAG..HEAD")" == "" ]]; then
    TAG_MAJOR=$((LAST_TAG_MAJOR + 1))
    TAG_MINOR="0"
    TAG_PATCH="0"
elif [[ "$(git show --pretty=format:%s --grep '^MINOR' "$LAST_TAG..HEAD")" == "" ]]; then
    TAG_MAJOR=$((LAST_TAG_MAJOR + 0))
    TAG_MINOR=$((LAST_TAG_MINOR + 1))
    TAG_PATCH="0"
elif [[ "$(git show --pretty=format:%s --grep '^PATCH' "$LAST_TAG..HEAD")" == "" ]]; then
    TAG_MAJOR=$((LAST_TAG_MAJOR + 0))
    TAG_MINOR=$((LAST_TAG_MINOR + 0))
    TAG_PATCH=$((LAST_TAG_PATCH + 1))
else
    WARNINGS="$WARNINGS ; This release is created with default bump version because no commits was ok"
    TAG_MAJOR=$((LAST_TAG_MAJOR + 0))
    TAG_MINOR=$((LAST_TAG_MINOR + 0))
    TAG_PATCH=$((LAST_TAG_PATCH + 1))
fi

TAG="$TAG_MAJOR.$TAG_MINOR.$TAG_PATCH"

gh release create "$TAG"           \
  --title "v$TAG"                  \
  --generate-notes                 \
  --target main

echo "release_tag=$TAG" >> $GITHUB_OUTPUT
echo "$WARNINGS"
