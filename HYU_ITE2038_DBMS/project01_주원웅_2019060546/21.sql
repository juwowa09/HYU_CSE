SELECT DISTINCT p.name
FROM raisingcharacter r1
JOIN raisingcharacter r2 on r1.cid = r2.cid
JOIN user u1 on u1.id = r1.owner_id
JOIN user u2 on u2.id = r2.owner_id
JOIN playablecharacter p on p.id = r1.cid
WHERE u1.country = 'Korea' and u2.country = 'UK'
ORDER BY p.name;