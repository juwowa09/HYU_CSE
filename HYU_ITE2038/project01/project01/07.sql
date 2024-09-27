SELECT DISTINCT p.name
FROM playableCharacter p
WHERE p.name REGEXP '^[ABCD]'
ORDER BY p.name;