class Equation 
  def self.solve_quadratic(a, b, c)

  		#reseni rovnice pokud je nektera promenna rovna nule
		if (a == 0)
			if (b == 0)
				return nil
			else
				return [+(-c.to_f/b)]				
			end
		end

		#reseni diskriminantu
		d = b*b-4*a*c 
		
		#reseni rovnice pokud je diskriminant roven nule
		if (d == 0)
		   	return [+(-b.to_f/2/a)]
		end

		#reseni rovnice pokud diskriminant neni roven nule
		if (d > 0)
		   	return [+((-b+Math.sqrt(d))/2/a),+((-b-Math.sqrt(d))/2/a)]
		end
  end
end