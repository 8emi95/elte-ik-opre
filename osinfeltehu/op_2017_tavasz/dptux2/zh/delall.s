#!/bin/bash
ipcs -m | grep "dptux2" |awk '{print $2}'| while read -r line; do ipcrm -m "$line";done
ipcs -q | grep "dptux2" |awk '{print $2}'| while read -r line; do ipcrm -q "$line";done
ipcs -s | grep "dptux2" |awk '{print $2}'| while read -r line; do ipcrm -s "$line";done