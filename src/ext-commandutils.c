#include "php.h"
#include "php_commandutils.h"
#include "Zend/zend_exceptions.h"

zend_class_entry *commandException_ce;
zend_class_entry *invalidCommandSyntaxException_ce;
zend_class_entry *commandStringHelper_ce;
zend_class_entry *assumptionFailedError_ce;

PHP_FUNCTION(parseQuoteAware) {
    char *command;
    size_t command_len;
    zval args;
    zend_string *pattern;
    regex_t regex;
    regmatch_t pmatch[3];

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(command, command_len)
    ZEND_PARSE_PARAMETERS_END();

    array_init(&args);
    
    pattern = zend_string_init("\"((?:\\\\.|[^\\\\\"])*)\"|(\\S+)", sizeof("\"((?:\\\\.|[^\\\\\"])*)\"|(\\S+)") - 1, 0);
    
    if (regcomp(&regex, ZSTR_VAL(pattern), REG_EXTENDED) != 0) {
        zend_string_release(pattern);
        RETURN_FALSE;
    }

    char *p = command;
    while (regexec(&regex, p, 3, pmatch, 0) == 0) {
        zend_string *match = zend_string_init(p + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so, 0);
        add_next_index_str(&args, match);
        p += pmatch[0].rm_eo;
    }

    regfree(&regex);
    zend_string_release(pattern);
    RETURN_ZVAL(&args, 1, 0);
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
