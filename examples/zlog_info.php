<?php

var_dump(zlog_init(__DIR__ . "/zlog.conf"));

var_dump(zlog_get_category("my_cat"));

zlog_info("hello world");
