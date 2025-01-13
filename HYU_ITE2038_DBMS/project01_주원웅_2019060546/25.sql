SELECT DISTINCT p.name 
FROM playablecharacter p
LEFT JOIN raisingcharacter r ON p.id = r.cid
WHERE r.cid IS NULL
ORDER BY p.name;
