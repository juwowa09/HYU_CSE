SELECT DISTINCT u.country
FROM user u
WHERE NOT u.name LIKE 'J%'
ORDER BY u.country;