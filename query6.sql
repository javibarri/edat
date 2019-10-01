select s1.*
from
	(select rental.customer_id, count(rental.inventory_id) as num, category.name  
		from category, film_category, film, rental, inventory
		where category.category_id = film_category.category_id and
		film_category.film_id = film.film_id and
		film.film_id = inventory.film_id and 
		inventory.inventory_id = rental.inventory_id
		group by customer_id, category.name
		order by num desc) as s1


	(select customer_id, count(inventory_id) as nums
		from rental
		group by customer_id
		order by nums desc) as s2


