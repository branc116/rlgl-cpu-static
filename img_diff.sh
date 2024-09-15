#!/usr/bin/env sh

set -xe

OLD=$(git status | rg modified | rg output | awk -p '{print $2}' | xargs echo)

cp $OLD new_output/

for i in ${OLD}
do
  git checkout ${i};
  imv ${i} $(echo ${i} | sed 's/output/new_output/')
  cp $(echo ${i} | sed 's/output/new_output/') ${i}
  rm $(echo ${i} | sed 's/output/new_output/')
done

