


    
DROP TABLE IF EXISTS `geo_ip_location`;

CREATE TABLE `geo_ip_location`(
	`id` INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,
	`country` CHAR(2) NOT NULL,
	`region` CHAR(2),
	`city` VARCHAR(32),
	`postal_code` VARCHAR(10),
	`latitude` DECIMAL(18,12),
	`longitude` DECIMAL(18,12),
	`metro_code` SMALLINT,
	`area_code` SMALLINT,
	PRIMARY KEY (`id`),
	INDEX `lat_long_index` ( `latitude`, `longitude` ),
	INDEX `country_region_city_index` ( `country`, `region`, `city` ),
	INDEX `postal_code_index` ( `postal_code` ),
	INDEX `area_code_index` ( `area_code` )
) ENGINE=MyISAM;



DROP TABLE IF EXISTS `geo_ip_block`;

CREATE TABLE `geo_ip_block`(
	`start_ip` MEDIUMINT UNSIGNED NOT NULL,
	`end_ip` MEDIUMINT UNSIGNED NOT NULL,
	`location_id` INTEGER UNSIGNED NOT NULL,
	INDEX `ip_range_index` ( `start_ip`, `end_ip` ),
	INDEX `location_index` ( `location_id` )
) ENGINE=MyISAM;




