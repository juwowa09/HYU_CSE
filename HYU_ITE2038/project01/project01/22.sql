SELECT DISTINCT u.name
FROM user u
JOIN raisingcharacter r on u.id = r.owner_id
JOIN playablecharacter p on r.cid = p.id
WHERE p.name LIKE 'n%' and u.country = 'Korea'
ORDER BY u.name;