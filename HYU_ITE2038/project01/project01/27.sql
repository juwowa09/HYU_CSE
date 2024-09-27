SELECT DISTINCT p.name
FROM playablecharacter p
WHERE (p.class = 'Nova' or p.class = 'Resistance') and p.branch = 'Warrior'
ORDER BY p.name;