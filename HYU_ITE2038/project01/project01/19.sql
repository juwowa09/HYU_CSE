SELECT u.name, SUM(r.level) sum_level
FROM user u
JOIN raisingcharacter r on r.owner_id = u.id
JOIN youtuber y on y.youtuber_id = u.id
GROUP BY u.id
ORDER BY SUM(r.level) DESC