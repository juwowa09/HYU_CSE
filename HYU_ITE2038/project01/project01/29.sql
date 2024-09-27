SELECT u.name
FROM user u
JOIN raisingcharacter r on r.owner_id = u.id
JOIN playablecharacter p on r.cid = p.id
WHERE p.branch NOT in (SELECT branch FROM playablecharacter p2 WHERE class = 'Resistance')
GROUP BY u.id
ORDER BY count(*)
LIMIT 1;