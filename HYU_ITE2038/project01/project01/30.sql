SELECT MIN(r.level) min_level_max_user
FROM raisingcharacter r 
JOIN user u on u.id = r.owner_id 
GROUP BY u.id 
ORDER BY MAX(r.level) DESC 
LIMIT 1;