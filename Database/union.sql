ALTER TABLE `characters` ADD `unionid` DECIMAL( 2 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `clanid` ,
ADD `union1points` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `unionid` ,
ADD `union2points` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `union1points` ,
ADD `union3points` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `union2points` ,
ADD `union4points` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `union3points` ,
ADD `union5points` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `union4points` ,
ADD `union6points` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `union5points` ,
ADD `union7points` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `union6points` ,
ADD `union8points` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `union7points` ,
ADD `union9points` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `union8points` ,
ADD `union10points` INT( 11 ) UNSIGNED NOT NULL DEFAULT '0' AFTER `union9points` ;
