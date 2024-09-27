SELECT u.name
FROM user u
JOIN raisingcharacter r on r.owner_id = u.id
GROUP BY u.id
HAVING AVG(r.level)
ORDER BY AVG(r.level) DESC
LIMIT 1;