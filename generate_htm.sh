#!/bin/bash
pandoc -s -S --toc -c pandoc.css -A footer.html ReadMe.md -o ReadMe.htm 
