SELECT DISTINCT u.name, p.name
FROM user u
JOIN raisingcharacter r on u.id = r.owner_id
JOIN playablecharacter p on p.id = r.cid
ORDER BY u.name, p.name;