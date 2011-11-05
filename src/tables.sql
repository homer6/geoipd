
Lookup Queries:



    
set @ip_address = inet_aton( "211.38.137.33" );

set @location_id = ( SELECT location_id FROM geo_ip_block
WHERE
    @ip_address >= start_ip AND
    @ip_address <= end_ip
);

SELECT * FROM geo_ip_location
WHERE
    id = @location_id
;








SELECT country, region, city  FROM geo_ip_location
WHERE
    id = ( SELECT location_id FROM geo_ip_block
        WHERE
            inet_aton( "211.38.137.33" ) >= start_ip AND
            inet_aton( "211.38.137.33" ) <= end_ip
    )
;





SELECT country, region, city FROM geo_ip_location
WHERE
    id = ( SELECT location_id FROM geo_ip_block
        WHERE 3542518049 BETWEEN start_ip AND end_ip
    )
;
