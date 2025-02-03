#ifndef PHP_COMMANDUTILS_H
#define PHP_COMMANDUTILS_H

extern zend_module_entry commandutils_module_entry;
#define phpext_commandutils_ptr &commandutils_module_entry

PHP_MINIT_FUNCTION(commandutils);
PHP_FUNCTION(parseQuoteAware);

#endif
