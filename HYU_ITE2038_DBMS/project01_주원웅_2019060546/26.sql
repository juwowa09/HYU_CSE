SELECT MAX(r.level) as country_max_level
FROM user u
JOIN raisingcharacter r on r.owner_id = u.id
GROUP BY u.country
ORDER BY MAX(r.level) DESC;