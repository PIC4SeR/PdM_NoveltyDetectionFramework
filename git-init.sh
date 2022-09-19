#!/bin/bash


git init
git add /home/giuseppe/Documents/Projects/NDF/AIU/main.py /home/giuseppe/Documents/Projects/NDF/AgentSystem/MultiAgentSystem/*
git commit -m "first commit"
git branch -M master
git remote add origin "https://github.com/PIC4SeR/PdM_NoveltyDetectionFramework.git"
git push -u origin master:master
