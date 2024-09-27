SELECT DISTINCT r.nickname
FROM youtuber y 
JOIN raisingcharacter r on y.youtuber_id = r.owner_id
JOIN playablecharacter p on r.cid = p.id
WHERE y.country = 'Korea' and p.class = 'explorer'
ORDER BY r.nickname;