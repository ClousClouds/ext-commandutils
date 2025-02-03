#include "php.h"
#include "php_commandutils.h"
#include "Zend/zend_exceptions.h"
#include "ext/pcre/php_pcre.h"

zend_class_entry *commandException_ce;
zend_class_entry *invalidCommandSyntaxException_ce;
zend_class_entry *commandStringHelper_ce;
zend_class_entry *assumptionFailedError_ce;

PHP_FUNCTION(parseQuoteAware) {
    char *command;
    size_t command_len;
    zend_string *pattern;
    zend_long flags = 0;
    zend_string *delimiter = zend_string_init("/", 1, 0);
    zend_string *compiled_pattern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(command, command_len)
    ZEND_PARSE_PARAMETERS_END();

    pattern = zend_string_init("\"((?:\\\\.|[^\\\\\"])*)\"|(\\S+)", strlen("\"((?:\\\\.|[^\\\\\"])*)\"|(\\S+)"), 0);
    compiled_pattern = zend_pcre_compile(pattern, delimiter, &flags, 0);
    
    if (!compiled_pattern) {
        RETURN_FALSE;
    }

    array_init(return_value);
    zend_string_release(pattern);
    zend_string_release(compiled_pattern);
}

static const zend_function_entry commandStringHelper_functions[] = {
    PHP_ME_MAPPING(parseQuoteAware, parseQuoteAware, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(commandutils) {
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "pocketmine\\command\\utils\\CommandException", NULL);
    commandException_ce = zend_register_internal_class_ex(&ce, zend_ce_exception);

    INIT_CLASS_ENTRY(ce, "pocketmine\\command\\utils\\InvalidCommandSyntaxException", NULL);
    invalidCommandSyntaxException_ce = zend_register_internal_class_ex(&ce, commandException_ce);

    INIT_CLASS_ENTRY(ce, "pocketmine\\command\\utils\\CommandStringHelper", commandStringHelper_functions);
    commandStringHelper_ce = zend_register_internal_class(&ce);

    INIT_CLASS_ENTRY(ce, "pocketmine\\utils\\AssumptionFailedError", NULL);
    assumptionFailedError_ce = zend_register_internal_class_ex(&ce, zend_ce_exception);

    return SUCCESS;
}

const zend_function_entry commandutils_functions[] = {
    PHP_FE(parseQuoteAware, NULL)
    PHP_FE_END
};

zend_module_entry commandutils_module_entry = {
    STANDARD_MODULE_HEADER,
    "commandutils",
    commandutils_functions,
    PHP_MINIT(commandutils),
    NULL,
    NULL,
    NULL,
    NULL,
    "0.1",
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_COMMANDUTILS
ZEND_GET_MODULE(commandutils)
#endif
