/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_zlog.h"
#include <zlog.h>

zlog_category_t *latest_c;
int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

/* If you declare any globals in php_zlog.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(zlog)
*/

/* True global resources - no need for thread safety here */
static int le_zlog;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("zlog.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_zlog_globals, zlog_globals)
    STD_PHP_INI_ENTRY("zlog.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_zlog_globals, zlog_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */

PHP_FUNCTION(zlog_init)
{
	char *config_file = NULL;
	size_t config_file_len;
	int rc;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &config_file, &config_file_len) == FAILURE) {
		return;
	}

	// check config exists
	if (!is_regular_file(config_file)) {
		char message[256];
		sprintf(message, "config path not found: %s", config_file);
		zend_throw_exception(NULL, message);
		RETURN_FALSE;
	}

	rc = zlog_init(config_file);

	if (rc) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}

PHP_FUNCTION(zlog_get_category)
{
	char *zlog_category_string = NULL;
	size_t zlog_category_string_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &zlog_category_string, &zlog_category_string_len) == FAILURE) {
		return;
	}

	zlog_category_t *c;
	c = latest_c =  zlog_get_category(zlog_category_string);
	if (!c) {
		php_printf("get category failed");
		zlog_fini();
		RETURN_FALSE;
	}

	zend_resource *zlog_category_resource;
	zlog_category_resource = zend_register_resource(c, le_zlog_category);

	RETURN_RES(zlog_category_resource);
}

PHP_FUNCTION(zlog_info)
{
	zval *c_resource_zval = NULL;
	char *log = NULL;
	size_t log_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|r", &log, &log_len, &c_resource_zval) == FAILURE) {
		return;
	}

	zlog_category_t *c;
	if (c_resource_zval) {
		c = Z_RES_VAL_P(c_resource_zval);
	} else if (latest_c) {
		c = latest_c;
	} else {
		zend_throw_exception(NULL, "no available zlog category");
		return;
	}

	zlog_info(c, log);

	RETURN_NULL();
}
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_zlog_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_zlog_init_globals(zend_zlog_globals *zlog_globals)
{
	zlog_globals->global_value = 0;
	zlog_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(zlog)
{
	le_zlog_category = zend_register_list_destructors_ex(NULL, NULL, "zlog_category", module_number);
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(zlog)
{
	zlog_fini();
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(zlog)
{
#if defined(COMPILE_DL_ZLOG) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(zlog)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(zlog)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "zlog support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ zlog_functions[]
 *
 * Every user visible function must have an entry in zlog_functions[].
 */
const zend_function_entry zlog_functions[] = {
	PHP_FE(zlog_init,	NULL)
	PHP_FE(zlog_get_category,	NULL)
	PHP_FE(zlog_info,	NULL)
	PHP_FE_END	/* Must be the last line in zlog_functions[] */
};
/* }}} */

/* {{{ zlog_module_entry
 */
zend_module_entry zlog_module_entry = {
	STANDARD_MODULE_HEADER,
	"zlog",
	zlog_functions,
	PHP_MINIT(zlog),
	PHP_MSHUTDOWN(zlog),
	PHP_RINIT(zlog),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(zlog),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(zlog),
	PHP_ZLOG_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ZLOG
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(zlog)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
