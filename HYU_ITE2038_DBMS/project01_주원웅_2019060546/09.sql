SELECT r.level
FROM user u
JOIN raisingcharacter r on u.id = r.owner_id
WHERE u.name = 'chang'
ORDER BY r.level DESC
LIMIT 1;