SELECT DISTINCT p.name 
FROM playablecharacter p 
WHERE branch = (SELECT branch FROM playablecharacter p2 WHERE name = 'Luminus')
ORDER BY p.name;