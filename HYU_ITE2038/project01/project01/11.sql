SELECT u.name, r.nickname
FROM raisingcharacter r
JOIN user u on u.id = r.owner_id
WHERE r.nickname LIKE 'j%'
ORDER BY u.name;