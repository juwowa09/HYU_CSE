SELECT r.owner_id, count(*) warrior_count 
FROM raisingcharacter r 
JOIN playablecharacter p on p.id = r.cid 
WHERE p.branch = 'warrior' 
GROUP BY r.owner_id 
ORDER BY count(*) DESC 
LIMIT 1;