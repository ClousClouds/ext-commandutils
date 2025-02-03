<?php

namespace pocketmine\command\utils;

/**
 * @internal
 */
class CommandException extends \RuntimeException {}

/**
 * @internal
 */
class InvalidCommandSyntaxException extends CommandException {}

/**
 * @internal
 */
class CommandStringHelper {
    /**
     * @param string $commandLine
     * @return string[]
     */
    public static function parseQuoteAware(string $commandLine) : array {}
}

namespace pocketmine\utils;

/**
 * @internal
 */
class AssumptionFailedError extends \Error {}
