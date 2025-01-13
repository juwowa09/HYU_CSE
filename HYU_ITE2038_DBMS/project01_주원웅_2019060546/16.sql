SELECT AVG(r.level) avg_Cygnus
FROM raisingcharacter r
JOIN playablecharacter p on r.cid = p.id
WHERE p.class = 'Cygnus_Knights';