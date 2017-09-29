require 'test/unit'
require_relative 'fib-sequence'

class FibonacciSequenceTest < Test::Unit::TestCase
  def setup
  	@seq = FibonacciSequence.new
  end

  #testovani metody next
  def test_next
    @seq.[](4)
    assert_equal(@seq.next,5)
  end

  #testovani metody current
  def test_current
    @seq.[](6)
    assert_equal(@seq.current,8)
  end

  #testovani metody current_idx
  def test_current_idx
    @seq.[](6)
    assert_equal(@seq.current_idx,6)
  end

  #testovani metody reset
  def test_reset
    @seq.reset
    assert_equal(@seq.current, nil)
    assert_equal(@seq.current_idx, nil)
    @seq.next
    assert_equal(@seq.current,0)
  end

  #testovani metody, ktera vraci fibonacciho cislo s danym indexem
  def test_indexace
  assert_equal(@seq.[](-3), nil)
  assert_equal(@seq.[](9), 34)
  end

end