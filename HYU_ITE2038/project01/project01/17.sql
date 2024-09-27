SELECT r.nickname
FROM youtuber y
JOIN raisingcharacter r on r.owner_id = y.youtuber_id
ORDER BY r.level DESC
LIMIT 1;